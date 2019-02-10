#include "rendererstorage.h"

#include <Tempest/Device>

#include "resources.h"

using namespace Tempest;

RendererStorage::RendererStorage(Tempest::Device &device)
  :device(device) {
  vsLand   = device.loadShader("shader/land.vert.sprv");
  fsLand   = device.loadShader("shader/land.frag.sprv");

  vsObject = device.loadShader("shader/object.vert.sprv");
  fsObject = device.loadShader("shader/object.frag.sprv");

  vsAni    = device.loadShader("shader/anim.vert.sprv");
  fsAni    = device.loadShader("shader/anim.frag.sprv");

  vsSky    = device.loadShader("shader/sky.vert.sprv");
  fsSky    = device.loadShader("shader/sky.frag.sprv");

  layoutLnd.add(0,Tempest::UniformsLayout::UboDyn, Tempest::UniformsLayout::Vertex);
  layoutLnd.add(1,Tempest::UniformsLayout::Texture,Tempest::UniformsLayout::Fragment);

  layoutObj.add(0,Tempest::UniformsLayout::Ubo,    Tempest::UniformsLayout::Vertex);
  layoutObj.add(1,Tempest::UniformsLayout::UboDyn, Tempest::UniformsLayout::Vertex);
  layoutObj.add(2,Tempest::UniformsLayout::Texture,Tempest::UniformsLayout::Fragment);

  layoutAni.add(0,Tempest::UniformsLayout::Ubo,    Tempest::UniformsLayout::Vertex);
  layoutAni.add(1,Tempest::UniformsLayout::UboDyn, Tempest::UniformsLayout::Vertex);
  layoutAni.add(2,Tempest::UniformsLayout::Texture,Tempest::UniformsLayout::Fragment);

  layoutSky.add(0,Tempest::UniformsLayout::UboDyn, Tempest::UniformsLayout::Fragment);
  layoutSky.add(1,Tempest::UniformsLayout::Texture,Tempest::UniformsLayout::Fragment);
  layoutSky.add(2,Tempest::UniformsLayout::Texture,Tempest::UniformsLayout::Fragment);
  }

void RendererStorage::initPipeline(Tempest::RenderPass &pass, uint32_t w, uint32_t h) {
  if((pLand.w()==w && pLand.h()==h) || w==0 || h==0)
    return;
  renderPass=&pass;

  RenderState stateAlpha;
  stateAlpha.setBlendSource (RenderState::BlendMode::src_alpha);
  stateAlpha.setBlendDest   (RenderState::BlendMode::one_minus_src_alpha);
  stateAlpha.setZTestMode   (RenderState::ZTestMode::Less);
  stateAlpha.setCullFaceMode(RenderState::CullMode::Front);

  RenderState stateObj;
  stateObj.setZTestMode   (RenderState::ZTestMode::Less);
  stateObj.setCullFaceMode(RenderState::CullMode::Front);

  RenderState stateLnd;
  stateLnd.setZTestMode   (RenderState::ZTestMode::Less);
  stateLnd.setCullFaceMode(RenderState::CullMode::Front);

  RenderState stateSky;
  stateSky.setZTestMode   (RenderState::ZTestMode::Always);
  stateSky.setCullFaceMode(RenderState::CullMode::Front);

  pLandAlpha = device.pipeline<Resources::Vertex>   (pass,w,h,Triangles,stateAlpha,layoutLnd,vsLand,  fsLand  );
  pLand      = device.pipeline<Resources::Vertex>   (pass,w,h,Triangles,stateLnd,  layoutLnd,vsLand,  fsLand  );

  pObject = device.pipeline<Resources::Vertex>   (pass,w,h,Triangles,stateObj,layoutObj,vsObject,fsObject);
  pAnim   = device.pipeline<Resources::VertexA>  (pass,w,h,Triangles,stateObj,layoutAni,vsAni,   fsAni   );
  pSky    = device.pipeline<Resources::VertexFsq>(pass,w,h,Triangles,stateSky,layoutSky,vsSky,   fsSky   );
  }
